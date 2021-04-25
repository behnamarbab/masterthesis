"""Integration code for Waffle fuzzer."""

import json
import os
import shutil
import subprocess

from fuzzers import utils


def prepare_build_environment():
    """Set environment variables used to build targets for AFL-based
    fuzzers."""
    cflags = ['-fsanitize-coverage=trace-pc-guard']
    utils.append_flags('CFLAGS', cflags)
    utils.append_flags('CXXFLAGS', cflags)

    os.environ['CC'] = 'clang'
    os.environ['CXX'] = 'clang++'
    os.environ['FUZZER_LIB'] = '/libAFL.a'


def build():
    """Build benchmark."""
    prepare_build_environment()

    utils.build_benchmark()

    print('[post_build] Copying waffle-fuzz to $OUT directory')
    # Copy out the waffle-fuzz binary as a build artifact.
    shutil.copy('/source_files/waffle/waffle-fuzz', os.environ['OUT'])


def get_stats(output_corpus, fuzzer_log):  # pylint: disable=unused-argument
    """Gets fuzzer stats for Waffle."""
    # Get a dictionary containing the stats Waffle reports.
    stats_file = os.path.join(output_corpus, 'fuzzer_stats')
    with open(stats_file) as file_handle:
        stats_file_lines = file_handle.read().splitlines()
    stats_file_dict = {}
    for stats_line in stats_file_lines:
        key, value = stats_line.split(': ')
        stats_file_dict[key.strip()] = value.strip()

    # Report to FuzzBench the stats it accepts.
    stats = {'execs_per_sec': float(stats_file_dict['execs_per_sec'])}
    return json.dumps(stats)


def prepare_fuzz_environment(input_corpus):
    """Prepare to fuzz with AFL or another AFL-based fuzzer."""
    # Tell AFL to not use its terminal UI so we get usable logs.
    os.environ['AFL_NO_UI'] = '1'
    # Skip AFL's CPU frequency check (fails on Docker).
    os.environ['AFL_SKIP_CPUFREQ'] = '1'
    # No need to bind affinity to one core, Docker enforces 1 core usage.
    os.environ['AFL_NO_AFFINITY'] = '1'
    # AFL will abort on startup if the core pattern sends notifications to
    # external programs. We don't care about this.
    os.environ['AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES'] = '1'
    # Don't exit when crashes are found. This can happen when corpus from
    # OSS-Fuzz is used.
    os.environ['AFL_SKIP_CRASHES'] = '1'

    # AFL needs at least one non-empty seed to start.
    utils.create_seed_file_for_empty_corpus(input_corpus)


def run_waffle_fuzz(input_corpus,
                 output_corpus,
                 target_binary,
                 additional_flags=None,
                 hide_output=False):
    """Run the fuzzer"""
    # Spawn the waffle fuzzing process.
    print('[run_waffle_fuzz] Running target with waffle-fuzz')
    command = ['./waffle-fuzz', '-i', input_corpus, '-o', output_corpus, '-d', '-m', 'none', '-t', '1000']
    if additional_flags:
        command.extend(additional_flags)
    dictionary_path = utils.get_dictionary_path(target_binary)
    if dictionary_path:
        command.extend(['-x', dictionary_path])
    command += [
        '--',
        target_binary,
        # Pass INT_MAX to afl the maximize the number of persistent loops it
        # performs.
        '2147483647'
    ]
    print('[run_waffle_fuzz] Running command: ' + ' '.join(command))
    output_stream = subprocess.DEVNULL if hide_output else None
    subprocess.check_call(command, stdout=output_stream, stderr=output_stream)


def fuzz(input_corpus, output_corpus, target_binary):
    """Run waffle-fuzz on target."""
    prepare_fuzz_environment(input_corpus)

    run_waffle_fuzz(input_corpus, output_corpus, target_binary)
